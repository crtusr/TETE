import tkinter as tk
from tkinter import ttk, messagebox
from dbfread import DBF

window = tk.Tk()

PRECIOB_var = tk.StringVar()

def autocomplete(event=None):
    current_text = producto_entry.get()
    suggestions = [row['NOMBRE'] for row in stock if row['NOMBRE'].startswith(current_text)]
    if suggestions:
        producto_entry.set('')  # Limpia el contenido actual
        producto_entry['values'] = suggestions  # Configura los nuevos valores
        producto_entry.set(suggestions[0])  # Configura el primer valor sugerido como el valor actual

        if isinstance(suggestions[0], dict):  # Verifica si el primer elemento es un diccionario
            PRECIOB_var.set(suggestions[0]['PRECIOB'])

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
        for row in stock:
            if row['NOMBRE'] == producto:
                precio = float(row['PRECIOB'])
                break
        total_producto = cantidad * precio
        factura_treeview.insert('', 'end', values=(producto, cantidad, precio, total_producto))
        calcular_total()
    except ValueError:
        messagebox.showerror("Error", "Por favor, introduce un número válido")

def calcular_total():
    total = 0.0
    for child in factura_treeview.get_children():
        total += float(factura_treeview.item(child, 'values')[3])
    total_label.config(text=f"Total: {total}")

stock = DBF('C:\\Users\\OEM\\Desktop\\CLIPPER TEST\\Python\\STOCK1.DBF', encoding='ANSI')

cliente_label = tk.Label(window, text="Nombre del Cliente")
cliente_label.pack()
cliente_entry = tk.Entry(window)
cliente_entry.pack()

producto_label = tk.Label(window, text="Producto")
producto_label.pack()
producto_entry = ttk.Combobox(window, postcommand=autocomplete)
producto_entry.pack()

cantidad_label = tk.Label(window, text="Cantidad")
cantidad_label.pack()
cantidad_entry = tk.Entry(window)
cantidad_entry.pack()

precio_label = tk.Label(window, text="Precio por unidad")
precio_label.pack()
precio_entry = tk.Entry(window)
precio_entry.pack()

agregar_button = tk.Button(window, text="Agregar producto", command=agregar_producto)
agregar_button.pack()

factura_treeview = ttk.Treeview(window, columns=('Producto', 'Cantidad', 'Precio', 'Total'), show='headings')
factura_treeview.heading('Producto', text='Producto')
factura_treeview.heading('Cantidad', text='Cantidad')
factura_treeview.heading('Precio', text='Precio')
factura_treeview.heading('Total', text='Total')
factura_treeview.pack()

total_label = tk.Label(window, text="Total: 0")
total_label.pack()

window.mainloop()

