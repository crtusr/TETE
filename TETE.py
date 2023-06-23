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
    "1. Clientes": clientes,
    "2. Stock": stock,
    "3. Facturación": facturacion,
    "4. Compras": compras,
    "5. Ctas.ctes.": ctas_ctes,
    "6. Parámetros": parametros,
    "7. Nota de crédito": nota_credito,
    "8. Remito": remito,
    "9. Registro de cheques": registro_cheques,
    "10. DOS": dos
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
def clientes():
    # Esta función debe permitir gestionar la ficha de clientes, usando widgets como cuadros de texto, etiquetas, botones, etc.
    pass

def stock():
    # Esta función debe permitir gestionar el stock de mercadería, usando widgets como listas, tablas, gráficos, etc.
    pass

def facturacion():
    pass
    
def compras():
    pass

def ctas_ctes():
    pass
    
def parametros():
    pass

def nota_credito():
    pass

def remito():
    pass

def registro_cheques():
    pass

def DOS():
    # Esta función debe cerrar la ventana principal y terminar el programa
    global app # Usar la variable global app que contiene la instancia de la clase principal
    app.destroy() # Destruir la ventana principal

# Llamar a la clase principal y a la función del menú principal desde el bloque if __name__ == "__main__"
if __name__ == "__main__":
    app = MainApp() # Crear una instancia de la clase principal y asignarla a la variable global app
    show_main_menu() # Llamar a la función que muestra el menú principal
    app.mainloop() # Llamar al método mainloop de la clase principal para iniciar el bucle principal del programa
