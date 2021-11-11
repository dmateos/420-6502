import optparse
import sys

parser = optparse.OptionParser()
parser.add_option("-f", "--file", dest="filename")
options, args = parser.parse_args()

if not options.filename:
    print("specify filename")
    sys.exit(1)

try:
    innerstr = ""
    with open(options.filename, mode="rb") as f:
        for byte in f.read():
            innerstr += f"{hex(byte)},"

    print(f"const uint8_t program[] = {{{innerstr}}};")
    sys.exit(0)
except FileNotFoundError:
    print("file not found")
    sys.exit(1)
