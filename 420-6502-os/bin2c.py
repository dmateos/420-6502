import optparse
import sys

parser = optparse.OptionParser()
parser.add_option("-f", "--file", dest="filename")

options, args = parser.parse_args()

if not options.filename:
    print("specify filename")
    sys.exit(1)

with open(options.filename, mode="rb") as f:
    file_data = f.read()
    print(file_data)

sys.exit(0)
