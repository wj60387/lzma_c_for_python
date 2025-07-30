import lzma_c 
original = b"ABCD"
compressed = lzma_c.compress(original)
print(f'compressed:{compressed.hex().upper()}')
decompressed = lzma_c.decompress(compressed)
print(f'decompressed:{decompressed}')
assert original == decompressed
print("Success!")
