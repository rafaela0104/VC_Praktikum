import cv2
import numpy as np

def color_transfer_reinhard(input_bgr, target_bgr):
    # 1) BGR → Lab (keine Normalisierung vorher!)
    input_lab = cv2.cvtColor(input_bgr, cv2.COLOR_BGR2Lab).astype(np.float32)
    target_lab = cv2.cvtColor(target_bgr, cv2.COLOR_BGR2Lab).astype(np.float32)

    result_lab = np.zeros_like(input_lab)

    for i in range(3):
        input_chan = input_lab[:, :, i]
        target_chan = target_lab[:, :, i]

        input_mean, input_std = cv2.meanStdDev(input_chan)
        target_mean, target_std = cv2.meanStdDev(target_chan)

        # Normalisieren
        normalized = (input_chan - input_mean[0][0]) / (input_std[0][0] + 1e-8)

        # Auf Ziel-Statistik skalieren
        scaled = normalized * target_std[0][0] + target_mean[0][0]

        result_lab[:, :, i] = scaled

    # Ergebnis clippen und in uint8 umwandeln
    result_lab = np.clip(result_lab, 0, 255).astype(np.uint8)

    # Zurück in BGR
    result_bgr = cv2.cvtColor(result_lab, cv2.COLOR_Lab2BGR)

    return result_bgr

# Beispielaufruf
input_img = cv2.imread("FigSource.png")
target_img = cv2.imread("FigTarget.png")

result = color_transfer_reinhard(input_img, target_img)

cv2.imshow("Input", input_img)
cv2.imshow("Target", target_img)
cv2.imshow("Farbtransfer Ergebnis", result)
cv2.waitKey(0)
cv2.destroyAllWindows()
