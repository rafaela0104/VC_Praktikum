import cv2
import numpy as np

# Bild laden
image = cv2.imread("yoshi.png")
if image is None:
    raise FileNotFoundError("Bild 'yoshi.png' nicht gefunden.")

# Bildgröße
height, width, channels = image.shape

# Quadrat einzeichnen
center_y, center_x = height // 2, width // 2
top_left = (center_x - 5, center_y - 5)
bottom_right = (center_x + 5, center_y + 5)
image_with_square = image.copy()
cv2.rectangle(image_with_square, top_left, bottom_right, (0, 0, 255), thickness=-1)

# Jede 5. Zeile schwarz
image_with_lines = image_with_square.copy()
image_with_lines[::5] = 0

# Bild anzeigen
cv2.imshow("Fertiges Bild", image_with_lines)
cv2.waitKey(0)
cv2.destroyAllWindows()

# Bild speichern
cv2.imwrite("yoshi_bearbeitet.png", image_with_lines)
print("Bild gespeichert als 'yoshi_bearbeitet.png'")

