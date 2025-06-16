import cv2
import numpy as np

# (1) Bilder laden
image_bgr = cv2.imread("yoshi.png")
mask = cv2.imread("mask.png", cv2.IMREAD_GRAYSCALE)

if image_bgr is None or mask is None:
    raise FileNotFoundError("Bild 'yoshi.png' oder 'mask.png' nicht gefunden.")

# (2) BGR → HSV
image_hsv = cv2.cvtColor(image_bgr, cv2.COLOR_BGR2HSV)


# (3) Trackbar-Update-Funktion
def update_hue(h_value):
    # Kopie des HSV-Bildes
    modified_hsv = image_hsv.copy()

    # (3) H-Wert für weiße Pixel in der Maske ändern
    modified_hsv[mask == 255, 0] = h_value

    # (4) Zurück in BGR für Anzeige
    modified_bgr = cv2.cvtColor(modified_hsv, cv2.COLOR_HSV2BGR)
    cv2.imshow("Yoshi mit geändertem H-Wert", modified_bgr)


cv2.namedWindow("Yoshi mit geändertem H-Wert", cv2.WINDOW_NORMAL)
#cv2.createTrackbar("H-Wert", "Yoshi mit geändertem H-Wert", 0, 179, update_hue)


# Initial anzeigen (Startwert = 0)
update_hue(90)

# Programm warten lassen, bis Taste gedrückt
cv2.waitKey(0)
cv2.destroyAllWindows()
