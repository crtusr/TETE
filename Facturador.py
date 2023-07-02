import tkinter as tk
from tkinter import ttk, messagebox
from dbfread import DBF
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import A4, A6
from reportlab.lib.units import cm
import os



window = tk.Tk()

window.title("Facturador")

PRECIOB_var = tk.StringVar()
TAMAÑO_var = tk.StringVar()

def autocomplete(event=None):
    current_text = producto_entry.get().lower()  # Convierte el texto de entrada a minúsculas
    suggestions = [row for row in stock if row['NOMBRE'].lower().startswith(current_text)]  # Convierte los nombres de los productos a minúsculas antes de hacer la comparación
    if suggestions:
        producto_entry.set('')  # Limpia el contenido actual
        suggestions_sorted = sorted(suggestions, key=lambda row: row['NOMBRE'])  # Ordena las sugerencias en orden alfabético
        producto_entry['values'] = [row['NOMBRE'] for row in suggestions_sorted]  # Configura los nuevos valores
        producto_entry.set(suggestions_sorted[0]['NOMBRE'])  # Configura el primer valor sugerido como el valor actual

        if isinstance(suggestions_sorted[0], dict):  # Verifica si el primer elemento es un diccionario
            PRECIOB_var.set(suggestions_sorted[0]['PRECIOB'])  # Actualiza el valor de PRECIOB_var
            TAMAÑO_var.set(suggestions_sorted[0]['ENVASE'])  # Actualiza el valor de PRECIOB_var


def robust_decode(bs):
    '''Intenta decodificar una cadena de bytes utilizando varias codificaciones.'''
    for encoding in ('utf-8', 'latin-1', 'cp1252'):  # Añade aquí más codificaciones si es necesario
        try:
            return bs.decode(encoding)
        except UnicodeDecodeError:
            pass
    raise UnicodeDecodeError('No se pudo decodificar la cadena de bytes')

def agregar_producto():
    try:
        producto = producto_entry.get()
        cantidad = float(cantidad_entry.get())
        producto = producto_entry.get()
        tamaño = tamaño_entry.get()  # Obtén el valor de tamaño_entry
        for row in stock:
            if row['NOMBRE'] == producto:
                precio = float(row['PRECIOB'])
                break
        total_producto = cantidad * precio
        factura_treeview.insert('', 'end', values=(producto, tamaño, cantidad, precio, total_producto))
        calcular_total()
        producto_entry.delete(0, tk.END)
        cantidad_entry.delete(0, tk.END)
        tamaño_entry.delete(0, tk.END)
    except ValueError:
        messagebox.showerror("Error", "Por favor, introduce un número válido")

def calcular_total():
    total = 0.0
    for child in factura_treeview.get_children():
        total += float(factura_treeview.item(child, 'values')[4])
    total_label.config(text=f"Total: {total}")

import subprocess

def create_and_print_invoice():
    # Crear un nuevo PDF con el tamaño de página A4
    c = canvas.Canvas("boleta.pdf", pagesize=A4)

    # Calcular los márgenes para centrar el A6 dentro del A4
    top_margin = A4[1] - A6[1]  # Margen superior para alinear con el borde de A4
    left_margin = (A4[0] - A6[0]) / 2  # Margen izquierdo para centrar

    # Configurar la fuente
    c.setFont("Helvetica", 11)

    # Extraer los datos del Treeview e imprimirlos en el PDF
    y = A6[1] + 400
    for row in factura_treeview.get_children():
        # Obtener los valores de la fila
        values = factura_treeview.item(row, 'values')

        # Imprimir los valores en el PDF
        c.drawString(left_margin + 10, y, str(values[0]))  # Producto
        c.drawRightString(left_margin + 150, y, str(values[1]))  # Cantidad
        c.drawRightString(left_margin + 190, y, str(values[2]))  # Precio
        c.drawRightString(left_margin + 230, y, str(values[3]))  # Total
        c.drawRightString(left_margin + 280, y, str(values[4]))  # Total

        # Mover la posición y hacia abajo para la siguiente fila
        y -= 20

    # Imprimir el total
    total = total_label.cget("text")  # Obtener el texto del total
    c.drawString(left_margin + 10, y, total)

    # Finalizar y guardar el PDF
    c.save()

    # Abrir el PDF con el lector de PDF predeterminado
    subprocess.Popen(["boleta.pdf"], shell=True)
    
    # Imprimir el PDF
    # os.startfile("boleta.pdf", "print")


import os
from dbfread import DBF

current_dir = os.path.dirname(os.path.realpath(__file__))
dbf_file = os.path.join(current_dir, 'STOCK1.DBF')

stock = DBF(dbf_file, encoding='ANSI')

cliente_label = tk.Label(window, text="Nombre del Cliente")
cliente_label.pack()
cliente_entry = tk.Entry(window)
cliente_entry.pack()

producto_label = tk.Label(window, text="Producto")
producto_label.pack()
producto_entry = ttk.Combobox(window, postcommand=autocomplete)
producto_entry.pack()

tamaño_label = tk.Label(window, text="Tamaño")
tamaño_label.pack()
tamaño_entry = tk.Entry(window, textvariable=TAMAÑO_var)
tamaño_entry.pack()

def on_product_selection(event):
    # Obtiene el producto seleccionado
    selected_product = producto_entry.get()

    # Busca el producto seleccionado en el stock y actualiza el precio
    for row in stock:
        if row['NOMBRE'] == selected_product:
            PRECIOB_var.set(row['PRECIOB'])
            TAMAÑO_var.set(row['ENVASE'])
            break

# Vincula el evento de selección de la lista de sugerencias a la función on_product_selection
producto_entry.bind('<<ComboboxSelected>>', on_product_selection)

cantidad_label = tk.Label(window, text="Cantidad")
cantidad_label.pack()
cantidad_entry = tk.Entry(window)
cantidad_entry.pack()

precio_label = tk.Label(window, text="Precio por unidad")
precio_label.pack()
precio_entry = tk.Entry(window, textvariable=PRECIOB_var)
precio_entry.pack()

agregar_button = tk.Button(window, text="Agregar producto", command=agregar_producto)
agregar_button.pack()

factura_treeview = ttk.Treeview(window, columns=('Producto', 'Tamaño', 'Cantidad', 'Precio', 'Total'), show='headings')
factura_treeview.heading('Producto', text='Producto')
factura_treeview.heading('Tamaño', text='Tamaño')
factura_treeview.heading('Cantidad', text='Cantidad')
factura_treeview.heading('Precio', text='Precio')
factura_treeview.heading('Total', text='Total')
factura_treeview.pack()

total_label = tk.Label(window, text="Total: 0")
total_label.pack()

print_button = tk.Button(window, text="Imprimir boleta", command=create_and_print_invoice)
print_button.pack()

window.mainloop()
