# pip install pystray pyserial Pillow
import serial
from pystray import Icon, MenuItem, Menu
from PIL import Image, ImageDraw
import sys
import threading # test -> check les performances du pc si ca le destroy aps par exemple avec un stress test CPU 

# Initialisation  port série
ser = None
server_on = False

# icône de couleur donnée
def create_icon(color):
    image = Image.new('RGB', (64, 64), color)
    draw = ImageDraw.Draw(image)
    draw.rectangle([0, 0, 64, 64], fill=color)
    return image

# Fonction pour allumer le serveur
def turn_on_server(icon):
    global ser, server_on
    try:
        # b'TURN ON YELLOW LED' b'TURN ON SERVER'
        ser.write(b'TURN ON YELLOW LED')
        response = ser.readline().decode('utf-8')
        print(f"Réponse du serveur: {response}")
        icon.icon = create_icon("green")
        server_on = True
    except Exception as e:
        print(f"Erreur lors de l'allumage du serveur : {e}")

# Fonction pour éteindre le serveur
def turn_off_server(icon):
    global ser, server_on
    try:
        #b'TURN OFF YELLOW LED'b'TURN OFF SERVER'
        ser.write(b'TURN OFF YELLOW LED')
        response = ser.readline().decode('utf-8')
        print(f"Réponse du serveur: {response}")
        icon.icon = create_icon("red")
        server_on = False
    except Exception as e:
        print(f"Erreur lors de l'extinction du serveur : {e}")

# initialiser la connexion série
def init_serial_connection():
    global ser
    try:
        ser = serial.Serial('COM3', baudrate=115200, timeout=1)
        print("Connexion série établie.")
    except Exception as e:
        print(f"Erreur lors de la connexion série : {e}")
        sys.exit(1)

# Fonction  fermer la connexion série 
def on_quit(icon, item):
    global ser
    if ser:
        ser.close()
        print("Connexion série fermée.")
    icon.stop()

# Menu 
menu = Menu(
    MenuItem('Allumer Serveur', turn_on_server, enabled=lambda item: not server_on),
    MenuItem('Éteindre Serveur', turn_off_server, enabled=lambda item: server_on),
    MenuItem('Quitter', on_quit)
)

# Création icône Pystray
icon = Icon('Serveur Control', icon=create_icon("red"), menu=menu)

# Thread pour exécuter l'icône en arrière-plan
def run_icon():
    icon.run()

# Initialisation  connexion série -> démarrer l'icône
if __name__ == "__main__":
    init_serial_connection()

    # Lancement icône dans un thread 
    threading.Thread(target=run_icon).start()
    
