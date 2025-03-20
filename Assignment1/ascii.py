import base64
import sys

information = sys.stdin.read().strip()

encoder = base64.a85encode(information.encode(), adobe=False)
print(encoder.decode() + "~>")
