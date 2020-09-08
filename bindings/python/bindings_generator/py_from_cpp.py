from template_generator import *
from bindings_generator import bindings_generator
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-t", "--template", nargs="+", help="Generates a brand new template file")
parser.add_argument("-b", "--build", nargs="+", help="Generates c++ bindings files")
args = vars(parser.parse_args())

if args['template'] is None and args['build'] is None:
    print("Invalid arguments, use --help flag to see the usage")
    exit(0)

if args['template'] is not None:
    for arg in args['template']:
        print("Created class template for {}".format(arg))
        tg = template_generator(arg)
        tg.create_class_template_file()

if args['build'] is not None:
    for arg in args['build']:
        print("{}.ct".format(arg))
        bg = bindings_generator("{}.ct".format(arg), arg)
        bg.generate_bindings()
