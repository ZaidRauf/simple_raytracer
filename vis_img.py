import matplotlib.pyplot as plt

# f = open("img")

with open('img', 'r') as f:
    width = int(f.readline().split(' ')[1].strip('\n'))
    height = int(f.readline().split(' ')[1].strip('\n'))
    print(width, height)

    img_data = []

    for line in f:
        pixel_data = (int(line.strip('\n')))

        pixel = [pixel_data >> 24, (pixel_data >> 16) & 0xFF, (pixel_data >> 8) & 0xFF]

        img_data.append(pixel)

    img = []

    for row in range(height):
        row_data = []

        for col in range(width):
            idx = col + (row * width)
            row_data.append(img_data[idx])

        img.append(row_data)

    plt.imshow(img)
    plt.show()
# f.close()