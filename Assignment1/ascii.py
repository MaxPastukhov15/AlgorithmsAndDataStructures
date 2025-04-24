import base64

d = input()

encoder = base64.a85encode(d.encode())  
decode = base64.a85decode(encoder)  

print(d, encoder, decode.decode())  

