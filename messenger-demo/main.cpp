#if defined(NDEBUG) && defined(_WIN32)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif // !NDEBUG

#include <window/ClassicWindow.h>
#include <widgets/AllWidgets.h>
#include <mcx/McxEngine.h>
using namespace mc;

#include "tcp_friend/tcp_friend.hpp"
using namespace tcp_friend;

#define SERVER_PORT 4554
const std::string serverIp = "66.65.180.12";

Shared<tcp_connection_socket> g_connection;
std::string g_username;

Shared<ScrollPanel> g_chatPanel = nullptr;

Shared<Label> createMessageLabel(const std::string& msg) {
	auto msgLbl = MakeRef<Label>();
	msgLbl->marginLeft = 20;
	msgLbl->marginRight = 20;
	msgLbl->fixedWidth = 460;
	msgLbl->alignment = "left";
	msgLbl->text = msg;
	return msgLbl;
}

void recordMessage(const std::string& msg) {
	g_chatPanel->addChild(createMessageLabel(msg));
}

int connectClient(const std::string& addr) {
    g_connection = tcp_client_socket::connect(addr, SERVER_PORT);
	if (!g_connection->is_alive()) {
		printf("Failed to connect to server on port %i\n", SERVER_PORT);
		return 1;
	}

    return 0;
}

void messageListeningLoop() {
	while (g_connection->is_alive()) {
		std::string msg;
		if (g_connection->recv_string(msg)) {
			recordMessage(msg);
		}
	}
}

int main() {
    AppManager::registerApplication("appId-messenger-demo");

    auto window = mcx::McxEngine::parseWindowFile("messenger_window.mcx");
	window->setBackgroundColor(Color(20, 20, 21));

	auto usernameEntry = std::static_pointer_cast<Entry>(getWidgetById("usernameEntry"));

	auto serverIpEntry = std::static_pointer_cast<Entry>(getWidgetById("serverIpEntry"));
	serverIpEntry->text = serverIp;

	g_chatPanel = std::static_pointer_cast<ScrollPanel>(getWidgetById("chatPanel"));
	recordMessage("Waiting to connect...");

	auto chatEntry = std::static_pointer_cast<Entry>(getWidgetById("chatEntry"));
	chatEntry->visible = false;
	chatEntry->on("entered", [&chatEntry](Shared<Event> e) {
		std::string msg = g_username + ":  " + e->get<std::string>("text");
		g_connection->send_string(msg);
		chatEntry->clear();
	});

	auto connectButton = getWidgetById("connectBtn");
    connectButton->on("clicked", [&usernameEntry, &serverIpEntry, &chatEntry](Shared<Event> e) {
		if (usernameEntry->text->empty()) {
			usernameEntry->borderColor = Color::red;
			return;
		}

        int status = connectClient(serverIpEntry->text.get());
		if (status == 0) {
			// Success
			chatEntry->visible = true;
			e->target->visible = false;
			getWidgetById("usernamePanel")->visible = false;

			g_username = usernameEntry->text;

			std::string connectionMessage = "* Connected to " + serverIpEntry->text.get() + " *";
			recordMessage(connectionMessage);

			std::thread messageListeningThread(messageListeningLoop);
			messageListeningThread.detach();
		} else {
			recordMessage("* Failed to connect to server, try again later... *");
		}
    });

    AppManager::startApplicationLoop();
    return 0;
}
