class template_generator:
    def __init__(self, class_name):
        self.class_name = class_name

    def create_class_template_file(self):
        with open("{}.ct".format(self.class_name), "w") as file:
            file.write("@class {}\n".format(self.class_name))
            file.write("@subclass_of null\n\n")

            # methods
            file.write("@class_methods_start\n")
            file.write("SampleMethod1\n")
            file.write("SampleMethod2\n")
            file.write("@class_methods_end\n\n")

            # getters
            file.write("@class_getters_start\n")
            file.write("SampleProperty1\n")
            file.write("SampleProperty2\n")
            file.write("@class_getters_end\n\n")

            # setters
            file.write("@class_setters_start\n")
            file.write("SampleProperty1\n")
            file.write("SampleProperty2\n")
            file.write("@class_setters_end\n\n")


