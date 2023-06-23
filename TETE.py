# Importar las bibliotecas necesarias
import tkinter as tk
import os
import sys

# Definir las variables globales
PATH = os.path.dirname(os.path.abspath(__file__)) # La ruta del directorio actual
COLOR = "lightblue" # El color de fondo de la ventana
FONT = ("Arial", 12) # La fuente y el tamaño de los textos

# Crear la clase principal que hereda de tkinter.Tk
class MainApp(tk.Tk):
    def __init__(self):
        super().__init__() # Llamar al constructor de la clase padre
        self.title("TETE - Sistema de gestión de viveros") # Poner el título de la ventana
        self.geometry("800x600") # Poner el tamaño de la ventana
        self.iconbitmap(os.path.join(PATH, "icono.ico")) # Poner el icono de la ventana
        self.config(bg=COLOR) # Poner el color de fondo de la ventana

    def mainloop(self):
        super().mainloop() # Llamar al método mainloop de la clase padre
        sys.exit(0) # Salir del programa al cerrar la ventana

# Crear una función que muestre el menú principal
def show_main_menu():
    global app # Usar la variable global app que contiene la instancia de la clase principal
    # Crear un diccionario con las opciones y sus funciones asociadas
    options = {
        "1. Ingresar datos": ingresar_datos,
        "2. Consultar datos": consultar_datos,
        "3. Modificar datos": modificar_datos,
        "4. Eliminar datos": eliminar_datos,
        "5. Salir": salir
    }
    # Crear un frame para contener los widgets del menú principal
    main_frame = tk.Frame(app, bg=COLOR)
    main_frame.pack(fill=tk.BOTH, expand=True) # Empaquetar el frame en la ventana principal
    # Crear un label con el título del menú principal
    title_label = tk.Label(main_frame, text="TETE - Sistema de gestión de viveros", font=("Arial", 24), bg=COLOR)
    title_label.pack(pady=20) # Empaquetar el label en el frame principal
    # Crear un bucle para recorrer las opciones y sus funciones
    for option, function in options.items():
        # Crear un button para cada opción con su texto y su función asociada
        option_button = tk.Button(main_frame, text=option, font=FONT, command=function)
        option_button.pack(pady=10) # Empaquetar el button en el frame principal

# Crear las funciones que correspondan a cada opción del menú principal (aquí solo pongo los nombres y comentarios)
def ingresar_datos():
    # Esta función debe permitir ingresar nuevos datos al sistema y guardarlos en la base de datos
    pass

def consultar_datos():
    # Esta función debe permitir consultar los datos existentes en el sistema y mostrarlos en la ventana
    pass

def modificar_datos():
    # Esta función debe permitir modificar los datos existentes en el sistema y actualizarlos en la base de datos
    pass

def eliminar_datos():
    # Esta función debe permitir eliminar los datos existentes en el sistema y borrarlos de la base de datos
    pass

def salir():
    # Esta función debe cerrar la ventana principal y terminar el programa
    global app # Usar la variable global app que contiene la instancia de la clase principal
    app.destroy() # Destruir la ventana principal

# Llamar a la clase principal y a la función del menú principal desde el bloque if __name__ == "__main__"
if __name__ == "__main__":
    app = MainApp() # Crear una instancia de la clase principal y asignarla a la variable global app
    show_main_menu() # Llamar a la función que muestra el menú principal
    app.mainloop() # Llamar al método mainloop de la clase principal para iniciar el bucle principal del programa
