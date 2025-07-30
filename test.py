import lzma_c 

original = b"ABCD"
compressed = lzma_c.compress(original)
print(f'input:{compressed.hex().upper()}')
decompressed = lzma_c.decompress(compressed)
print(f'output:{decompressed}')
assert original == decompressed
print("Success!")
