import optparse
import sys

parser = optparse.OptionParser()
parser.add_option("-f", "--file", dest="filename")
options, args = parser.parse_args()

if not options.filename:
    print("specify filename")
    sys.exit(1)

innerstr = ""
size = 0

try:
    with open(options.filename, mode="rb") as f:
        for byte in f.read():
            innerstr += f"{hex(byte)},"
            size += 1
except FileNotFoundError:
    pass

print(f"const uint8_t program[] = {{{innerstr}}};\nconst uint32_t program_size = {size};")
sys.exit(0)
