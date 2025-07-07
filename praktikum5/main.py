import cv2
import numpy as np
import matplotlib.pyplot as plt

# --- 1. BILD schrott.png laden und Graustufen-Kontrast-Transformation ---

image = cv2.imread("schrott.png", cv2.IMREAD_GRAYSCALE)
if image is None:
    raise FileNotFoundError("Bild 'schrott.png' konnte nicht geladen werden!")

def plot_histogram(img, title, cumulative=False, save_as=None):
    hist = cv2.calcHist([img], [0], None, [256], [0, 256]).flatten()
    if cumulative:
        hist = np.cumsum(hist)
    plt.figure()
    plt.title(title)
    plt.plot(hist)
    plt.xlim([0, 256])
    if save_as:
        plt.savefig(save_as)
    else:
        plt.show()
    plt.close()

min_val = np.min(image)
max_val = np.max(image)

low, high = 100, 150
reduced_contrast = ((image - min_val) / (max_val - min_val) * (high - low) + low).astype(np.uint8)
maximized_contrast = ((reduced_contrast - low) / (high - low) * 255).astype(np.uint8)

equalized_image = cv2.equalizeHist(image)
equalized_reduced = cv2.equalizeHist(reduced_contrast)

cv2.imwrite("reduced_contrast.png", reduced_contrast)
cv2.imwrite("maximized_contrast.png", maximized_contrast)
cv2.imwrite("equalized_image.png", equalized_image)
cv2.imwrite("equalized_reduced.png", equalized_reduced)

plot_histogram(image, "Originalbild Histogramm", save_as="original_hist.png")
plot_histogram(reduced_contrast, "Reduzierter Kontrast [100–150]", save_as="reduced_hist.png")
plot_histogram(maximized_contrast, "Maximierter Kontrast [0–255]", save_as="maximized_hist.png")
plot_histogram(equalized_image, "Histogramm nach Equalization (Originalbild)", save_as="equalized_orig_hist.png")
plot_histogram(equalized_reduced, "Histogramm nach Equalization (reduzierter Kontrast)", save_as="equalized_reduced_hist.png")

# --- 2. BILD kante.png laden und Faltung mit filter2D ---

kante = cv2.imread("kante.png", cv2.IMREAD_GRAYSCALE)
if kante is None:
    raise FileNotFoundError("Bild 'kante.png' konnte nicht geladen werden!")

# Beispiel-Faltungsmatrizen (bitte ggf. anpassen oder durch Vorgabe ersetzen)

F1 = np.array([[0, -1, 0],
               [-1, 4, -1],
               [0, -1, 0]], dtype=np.float32)  # Laplace-Kante, Summe=0

F2 = np.ones((3,3), dtype=np.float32) / 9  # Mittelwertfilter, Summe=1

F3 = np.array([[-1, -1, -1],
               [-1, 8, -1],
               [-1, -1, -1]], dtype=np.float32)  # Schärfungsfilter, Summe=0

# Faltung F1 mit delta=128 (wegen Summe=0)
filtered_F1 = cv2.filter2D(kante, ddepth=-1, kernel=F1, delta=128, borderType=cv2.BORDER_DEFAULT)

# Auf Ergebnis F1 F2 anwenden mit delta=128
filtered_F1_F2 = cv2.filter2D(filtered_F1, ddepth=-1, kernel=F2, delta=128, borderType=cv2.BORDER_DEFAULT)

# Auf Original F3 anwenden mit delta=128
filtered_F3 = cv2.filter2D(kante, ddepth=-1, kernel=F3, delta=128, borderType=cv2.BORDER_DEFAULT)

# Ergebnisse speichern
cv2.imwrite("kante_filtered_F1.png", filtered_F1)
cv2.imwrite("kante_filtered_F1_F2.png", filtered_F1_F2)
cv2.imwrite("kante_filtered_F3.png", filtered_F3)

# Ergebnisse anzeigen (optional)
cv2.imshow("Original kante.png", kante)
cv2.imshow("Faltung F1 + 128", filtered_F1)
cv2.imshow("Faltung F1 dann F2 + 128", filtered_F1_F2)
cv2.imshow("Faltung F3 + 128", filtered_F3)
cv2.waitKey(0)
cv2.destroyAllWindows()

# -------------------
# Fragen & Antworten:
# -------------------

# 1) Was liefert der Vergleich der beiden zuletzt erzeugten Ergebnisse?
#    (filtered_F1 dann filtered_F2 vs. filtered_F3)
#
# Antwort:
# Die Faltung mit F1 detektiert Kanten (ähnlich Laplace-Operator) und mit F2 wird das Ergebnis geglättet.
# Das Ergebnis von F1+F2 ist ein weicheres Kantenbild. F3 ist ein Schärfungsfilter, der Kanten stärker hervorhebt.
# Somit wirkt das Bild von F3 kontrastreicher und detailreicher an Kanten als das geglättete F1+F2.

# 2) Die Faltungsmatrizen F1 und F2 differenzieren in Zeilenrichtung (Ableitung).
#    Was bewirkt F3 bzgl. "Ableitung"?
#
# Antwort:
# F3 entspricht einem Laplace-Operator (zweite Ableitung in x- und y-Richtung).
# Er hebt lokale Helligkeitsänderungen hervor und zeigt Bereiche mit starken Kanten und Übergängen,
# nicht nur in einer Richtung, sondern in allen Richtungen.

# 3) Was liefert der pixelweise Vergleich des Ergebnisses der Faltung mit F6 mit dem Ergebnis der Faltung mit F3?
#
# Antwort:
# F6 = F4 - F5 ist die Differenz zweier erster Ableitungen in x-Richtung (gerichtete Kanten).
# F3 ist der Laplace-Operator, der zweite Ableitungen in beiden Richtungen berechnet.
# Daher unterscheiden sich die Ergebnisse: F6 zeigt vor allem horizontale Kanten,
# F3 erfasst Kanten in allen Richtungen und ist sensitiv gegenüber stärkerem Kantenverlauf.

# 4) Was bewirken die beiden Faltungen F4 bzw. F5?
#
# Antwort:
# F4 und F5 sind erste Ableitungen in der x-Richtung, aber mit unterschiedlicher Polarität.
# F4 erkennt Übergänge von dunkler zu heller Fläche von links nach rechts,
# F5 erkennt Kanten von rechts nach links.
# Zusammen erfassen sie die Richtung und Stärke von horizontalen Kanten.

# 5) Was erhält man, wenn man die Faltungsmatrix F3 um 90 Grad dreht und zu F3 selbst addiert?
#    Wie heißt dieser Operator und was bewirkt er bzgl. "Ableitung"?
#
# Antwort:
# Die Drehung von F3 um 90 Grad und Addition zu F3 erzeugt einen isotropen Laplace-Operator,
# der Kanten und Übergänge unabhängig von Richtung hervorhebt.
# Dies ist ein Operator für die zweite Ableitung (Laplacian),
# der lokale Bildkontraste und Kanten sehr präzise detektiert.

# -------------------
