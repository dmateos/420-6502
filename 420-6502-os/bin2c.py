import optparse
import sys

parser = optparse.OptionParser()
parser.add_option("-f", "--file", dest="filename")
options, args = parser.parse_args()

if not options.filename:
    print("specify filename")
    sys.exit(1)

innerstr = ""
with open(options.filename, mode="rb") as f:
    for byte in f.read():
        innerstr += f"{hex(byte)},"

print(f"const uint8_t kernel_program[] = {{{innerstr}}};")
sys.exit(0)
