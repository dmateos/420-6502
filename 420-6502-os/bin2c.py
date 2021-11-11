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
    size = 0
    with open(options.filename, mode="rb") as f:
        for byte in f.read():
            innerstr += f"{hex(byte)},"
            size += 1

    print(f"const uint8_t program[] = {{{innerstr}}};\nconst uint32_t program_size = {size};")
    sys.exit(0)
except FileNotFoundError:
    print("file not found")
    sys.exit(1)
