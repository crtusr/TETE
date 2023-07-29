import tkinter as tk
from tkinter import ttk, messagebox
from tkinter import Grid
from dbfread import DBF
from reportlab.pdfgen import canvas
from reportlab.lib.pagesizes import A4, A6
from reportlab.lib.units import cm
from datetime import datetime
import math
import os

window = tk.Tk()

window.title("Facturador")

PRECIOB_var = tk.StringVar()
precio_bonificado_var = tk.IntVar()
TAMAÑO_var = tk.StringVar()
numero_var = tk.StringVar()
acreedor_var = tk.StringVar()
deudor_var = tk.StringVar()
acreedor_deudor_var = tk.StringVar()
existencias_var = tk.StringVar()
codigo_var = tk.StringVar()
efectivo_var = tk.StringVar()
cheque_var = tk.StringVar()

def reset_program():
    producto_entry.delete(0, 'end')
    cliente_entry.delete(0, 'end')
    cantidad_entry.delete(0, 'end')
    precio_entry.delete(0, tk.END)
    existencia_entry.delete(0, tk.END)
    producto_entry.delete(0, tk.END)
    cantidad_entry.delete(0, tk.END)
    tamaño_entry.delete(0, tk.END)
    cliente_entry.delete(0, tk.END)
    boleta_entry.delete(0, tk.END)
    codi_entry.delete(0, tk.END)
    acreedor_deudor_entry.delete(0, tk.END)
    efectivo_entry.delete(0, tk.END)
    cheque_entry.delete(0, tk.END)
    numero_entry.delete(0, tk.END)

    # Clear the Treeview
    for item in factura_treeview.get_children():
        factura_treeview.delete(item)

    calcular_total()

reset_button = tk.Button(window, text="Nueva boleta", command=reset_program)
reset_button.grid(row=0, column=5)

def update_price(*args):
    if var.get():
        # Aumenta el precio en un 30% y redondea hacia arriba a las decenas más cercanas
        new_price = math.ceil((float(precio_entry.get()) * 1.3) / 10.0) * 10
        precio_entry.delete(0, tk.END)
        precio_entry.insert(0, str(new_price))

def autocomplete(event=None):
    current_text = producto_entry.get().lower()  # Convert the input text to lowercase
    # Convert product names to lowercase before comparison
    suggestions = [row for row in stock if current_text in row['NOMBRE'].lower()]
    if suggestions:
        producto_entry.set('')  # Clear the current content
        suggestions_sorted = sorted(suggestions, key=lambda row: row['NOMBRE'])  # Sort the suggestions alphabetically

        # Concatenate the name and size in the suggestions
        suggestions_with_size = [f"{row['NOMBRE']}--{row['ENVASE']}" for row in suggestions_sorted]

        producto_entry['values'] = suggestions_with_size  # Set the new values
        #producto_entry.set(suggestions_with_size[0])  # Set the first suggestion as the current value

    cli_text = cliente_entry.get().lower()
    # Now it will also match substrings in the 'RAZON' field
    suggestions2 = [row for row in clipro if cli_text in row['RAZON'].lower()]
    if suggestions2:
        cliente_entry.set('')  # Clear the current content
        suggestions2_sorted = sorted(suggestions2, key=lambda row: row['RAZON'])  # Sort the suggestions alphabetically
        cliente_entry['values'] = [row['RAZON'] for row in suggestions2_sorted]  # Set the new values
        cliente_entry.set(suggestions2_sorted[0]['RAZON'])  # Set the first suggestion as the current value


def robust_decode(bs):
    '''Intenta decodificar una cadena de bytes utilizando varias
codificaciones.'''
    for encoding in ('utf-8', 'latin-1', 'cp1252'):  # Añade aquí más codificaciones si es necesario
        try:
            return bs.decode(encoding)
        except UnicodeDecodeError:
            pass
    raise UnicodeDecodeError('No se pudo decodificar la cadena de bytes')

def agregar_producto():
    try:
        global precio
        cantidad = float(cantidad_entry.get())
        producto = producto_entry.get()
        tamaño = tamaño_entry.get()  # Obtén el valor de tamaño_entry
        precio = float(precio_entry.get())
        total_producto = float(cantidad) * float(precio)
        factura_treeview.insert('', 'end', values=(producto, tamaño, cantidad, precio, total_producto))
        calcular_total()
        precio_entry.delete(0, tk.END)
        existencia_entry.delete(0, tk.END)
        producto_entry.delete(0, tk.END)
        cantidad_entry.delete(0, tk.END)
        tamaño_entry.delete(0, tk.END)
        producto_entry.focus_set()  # Mueve el cursor a la casilla producto_entry
    except ValueError:
        messagebox.showerror("Error", "Por favor, introduce un número válido")

def calcular_total():
    total = 0.0
    totalisimo = 0.0
    pagoe = 0.0
    pagoch = 0.0
    saldo = acreedor_deudor_var.get()
    saldo = '0' if saldo == '' else saldo
    saldot = 0.0
    for child in factura_treeview.get_children():
        total += float(factura_treeview.item(child, 'values')[4])
    total_label.config(text=f"Subtotal: {total}")
    totalisimo = float(total) - float(saldo)
    totalisimo_label.config(text=f"Total: {totalisimo}")
    pagoe = efectivo_var.get() # Get value from efectivo_var
    pagoch = cheque_var.get() # Get value from cheque_var
    pagoe = '0' if pagoe == '' else pagoe
    pagoch = '0' if pagoch == '' else pagoch
    pagot = float(pagoe) + float (pagoch)
    saldot = -float(totalisimo) + float(pagot)
    saldo_final_label.config(text=f"Saldo final: {saldot}")
def borrar_fila(event):
    selected = factura_treeview.selection()  # Obtener los items seleccionados
    for item in selected:
        factura_treeview.delete(item)  # Eliminar el item del Treeview


import subprocess

def create_and_print_invoice():
    # Crear un nuevo PDF con el tamaño de página A4
    c = canvas.Canvas("boleta.pdf", pagesize=A4)

    # Calcular los márgenes para centrar el A6 dentro del A4
    top_margin = A4[1] - A6[1]  # Margen superior para alinear con el borde de A4
    left_margin = (A4[0] - A6[0]) / 2  # Margen izquierdo para centrar

    # Configurar la fuente
    c.setFont("Helvetica", 9)

    # Extraer los datos del Treeview e imprimirlos en el PDF
    y = A6[1] + 400

    c.drawRightString(left_margin + 280, y, "PX" + str(boleta_entry.get()))
    c.drawString(left_margin + 10, y, "Cliente: " + str(cliente_entry.get()) + " " + str(numero_entry.get()))

    y -= 20

    now = datetime.now()
    date_time_str = now.strftime("%d/%m/%Y %H:%M:%S")
    c.drawString(left_margin + 10, y, "Fecha y hora: " + date_time_str)
    page_number = 1
    c.drawRightString(left_margin + 280, y, "Página: " + str(page_number))

    y -= 20

    c.drawString(left_margin + 10, y, "Descripción")       # Producto
    c.drawRightString(left_margin + 190, y, "Cantidad")  # Precio
    c.drawRightString(left_margin + 230, y, "Precio")  # Total
    c.drawRightString(left_margin + 280, y, "Total")  # Total

    y -= 20

    # Inicializar la variable para la suma de los valores
    suma = 0
    suma_total = 0

    for row in factura_treeview.get_children():
        # Obtener los valores de la fila
        values = factura_treeview.item(row, 'values')

        # Imprimir los valores en el PDF
        c.drawString(left_margin + 10, y, str(values[0]))       # Producto
        c.drawRightString(left_margin + 190, y, str(values[2]))  # Cantidad
        c.drawRightString(left_margin + 230, y, "{:.2f}".format(float(values[3])))  # Precio
        c.drawRightString(left_margin + 280, y, "{:.2f}".format(float(values[4])))  # Total

        # Sumar el valor a la variable suma
        suma += float(values[4])
        suma_total += float(values[4])

        # Mover la posición y hacia abajo para la siguiente fila
        y -= 13

        if y < A6[1] + 70:
            # Imprimir la suma de los valores en el final de la página
            if page_number > 1:
                y -=7
                c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))
                y -= 13
                c.drawRightString(left_margin + 280, y, "Suma de paginas anteriores: {:.2f}".format(suma_total - suma))
                y -= 13
                c.drawRightString(left_margin + 280, y, "Subtotal: {:.2f}".format(suma_total))

            else:
                y -=7
                c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))

            c.showPage()
            c.setFont("Helvetica", 9)
            y = A6[1] + 400

            c.drawRightString(left_margin + 280, y, "PX" + str(boleta_entry.get()))
            c.drawString(left_margin + 10, y, "Cliente: " + str(cliente_entry.get()) + " " + str(numero_entry.get()))

            y -= 20

            now = datetime.now()
            date_time_str = now.strftime("%d/%m/%Y %H:%M:%S")
            c.drawString(left_margin + 10, y, "Fecha y hora: " + date_time_str)
            page_number += 1
            c.drawRightString(left_margin + 280, y, "Página: " + str(page_number))

            y -= 20

            c.drawString(left_margin + 10, y, "Descripción")       # Producto
            c.drawRightString(left_margin + 190, y, "Cantidad")  # Precio
            c.drawRightString(left_margin + 230, y, "Precio")  # Total
            c.drawRightString(left_margin + 280, y, "Total")  # Total

            y -= 20

            # Reiniciar la variable suma para la nueva página
            suma = 0

        elif y < A6[1] + 90 and page_number > 1:

            # Imprimir la suma de los valores en el final de la página
            if page_number > 1:
                y -=7
                c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))
                y -= 13
                c.drawRightString(left_margin + 280, y, "Suma de paginas anteriores: {:.2f}".format(suma_total - suma))
                y -= 13
                c.drawRightString(left_margin + 280, y, "Subtotal: {:.2f}".format(suma_total))

            else:
                y -=7
                c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))

            c.showPage()
            c.setFont("Helvetica", 9)
            y = A6[1] + 400

            c.drawRightString(left_margin + 280, y, "PX" + str(boleta_entry.get()))
            c.drawString(left_margin + 10, y, "Cliente: " + str(cliente_entry.get()) + " " + str(numero_entry.get()))

            y -= 20

            now = datetime.now()
            date_time_str = now.strftime("%d/%m/%Y %H:%M:%S")
            c.drawString(left_margin + 10, y, "Fecha y hora: " + date_time_str)
            page_number += 1
            c.drawRightString(left_margin + 280, y, "Página: " + str(page_number))

            y -= 20

            c.drawString(left_margin + 10, y, "Descripción")       # Producto
            c.drawRightString(left_margin + 190, y, "Cantidad")  # Precio
            c.drawRightString(left_margin + 230, y, "Precio")  # Total
            c.drawRightString(left_margin + 280, y, "Total")  # Total

            y -= 20

            # Reiniciar la variable suma para la nueva página
            suma = 0

    if page_number > 1:

        y -=7

        c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))

        y -= 13

        c.drawRightString(left_margin + 280, y, "Suma de paginas anteriores: {:.2f}".format(suma_total - suma))

        y -= 13

    else:

        y -= 7
        
    
    total = total_label.cget("text")  # Obtener el texto del total
    total = total.split(" ")[1]  # Quitar la palabra "Subtotal:"
    c.drawRightString(left_margin + 280, y, "Subtotal {:.2f}".format(float(total)))  # Imprimir el total

    y -= 13

    saldo = acreedor_deudor_var.get()  # Obtener el texto del saldo
    saldo = '0' if saldo == '' else saldo
    c.drawRightString(left_margin + 280, y, "Saldo: {:.2f}".format(float(saldo)))  # Imprimir el saldo

    pago_ef = efectivo_var.get()  # Obtener el texto del pago en efectivo
    pago_ef = '0' if pago_ef == '' else pago_ef
    c.drawString(left_margin + 10, y, "Efectivo: {:.2f}".format(float(pago_ef)))  # Imprimir el pago en efectivo

    y -= 13

    pago_ch = cheque_var.get()  # Obtener el texto del pago con cheque
    pago_ch = '0' if pago_ch == '' else pago_ch
    c.drawString(left_margin + 10, y, "Cheque: {:.2f}".format(float(pago_ch)))  # Imprimir el pago con cheque

    totalisimo = totalisimo_label.cget("text")  # Obtener el texto del totalisimo
    totalisimo = totalisimo.split(" ")[1]  # Quitar la palabra "Total:"
    c.drawRightString(left_margin + 280, y, "Total {:.2f}".format(float(totalisimo)))  # Imprimir el totalisimo

    y -= 13

    saldo_fin = saldo_final_label.cget("text")  # Obtener el texto del saldo final
    saldo_fin = saldo_fin.split(" ")[2]  # Quitar la palabra "Total:"
    c.drawRightString(left_margin + 280, y, "Saldo final: {:.2f}".format(float(saldo_fin)))  # Imprimir el saldo final

    #######

    c.showPage()

    c.setFont("Helvetica", 9)

    # Extraer los datos del Treeview e imprimirlos en el PDF
    y = A6[1] + 400

    c.drawRightString(left_margin + 280, y, "PX" + str(boleta_entry.get()))
    c.drawString(left_margin + 10, y, "Cliente: " + str(cliente_entry.get()) + " " + str(numero_entry.get()))

    y -= 20

    now = datetime.now()
    date_time_str = now.strftime("%d/%m/%Y %H:%M:%S")
    c.drawString(left_margin + 10, y, "Fecha y hora: " + date_time_str)
    c.drawString(left_margin + 180, y, "DUPLICADO")
    page_number = 1
    c.drawRightString(left_margin + 280, y, "Página: " + str(page_number))

    y -= 20

    c.drawString(left_margin + 10, y, "Descripción")       # Producto
    c.drawRightString(left_margin + 190, y, "Cantidad")  # Precio
    c.drawRightString(left_margin + 230, y, "Precio")  # Total
    c.drawRightString(left_margin + 280, y, "Total")  # Total

    y -= 20

    # Inicializar la variable para la suma de los valores
    suma = 0
    suma_total = 0

    for row in factura_treeview.get_children():
        # Obtener los valores de la fila
        values = factura_treeview.item(row, 'values')

        # Imprimir los valores en el PDF
        c.drawString(left_margin + 10, y, str(values[0]))       # Producto
        c.drawRightString(left_margin + 190, y, str(values[2]))  # Cantidad
        c.drawRightString(left_margin + 230, y, "{:.2f}".format(float(values[3])))  # Precio
        c.drawRightString(left_margin + 280, y, "{:.2f}".format(float(values[4])))  # Total

        # Sumar el valor a la variable suma
        suma += float(values[4])
        suma_total += float(values[4])

        # Mover la posición y hacia abajo para la siguiente fila
        y -= 13

        if y < A6[1] + 70:
            # Imprimir la suma de los valores en el final de la página
            if page_number > 1:
                y -=7
                c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))
                y -= 13
                c.drawRightString(left_margin + 280, y, "Suma de paginas anteriores: {:.2f}".format(suma_total - suma))
                y -= 13
                c.drawRightString(left_margin + 280, y, "Subtotal: {:.2f}".format(suma_total))

            else:
                y -=7
                c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))

            c.showPage()
            c.setFont("Helvetica", 9)
            y = A6[1] + 400

            c.drawRightString(left_margin + 280, y, "PX" + str(boleta_entry.get()))
            c.drawString(left_margin + 10, y, "Cliente: " + str(cliente_entry.get()) + " " + str(numero_entry.get()))

            y -= 20

            now = datetime.now()
            date_time_str = now.strftime("%d/%m/%Y %H:%M:%S")
            c.drawString(left_margin + 10, y, "Fecha y hora: " + date_time_str)
            c.drawString(left_margin + 180, y, "DUPLICADO")
            page_number += 1
            c.drawRightString(left_margin + 280, y, "Página: " + str(page_number))

            y -= 20

            c.drawString(left_margin + 10, y, "Descripción")       # Producto
            c.drawRightString(left_margin + 190, y, "Cantidad")  # Precio
            c.drawRightString(left_margin + 230, y, "Precio")  # Total
            c.drawRightString(left_margin + 280, y, "Total")  # Total

            y -= 20

            # Reiniciar la variable suma para la nueva página
            suma = 0

        elif y < A6[1] + 90 and page_number > 1:

            # Imprimir la suma de los valores en el final de la página
            if page_number > 1:
                y -=7
                c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))
                y -= 13
                c.drawRightString(left_margin + 280, y, "Suma de paginas anteriores: {:.2f}".format(suma_total - suma))
                y -= 13
                c.drawRightString(left_margin + 280, y, "Subtotal: {:.2f}".format(suma_total))

            else:
                y -=7
                c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))

            c.showPage()
            c.setFont("Helvetica", 9)
            y = A6[1] + 400

            c.drawRightString(left_margin + 280, y, "PX" + str(boleta_entry.get()))
            c.drawString(left_margin + 10, y, "Cliente: " + str(cliente_entry.get()) + " " + str(numero_entry.get()))

            y -= 20

            now = datetime.now()
            date_time_str = now.strftime("%d/%m/%Y %H:%M:%S")
            c.drawString(left_margin + 10, y, "Fecha y hora: " + date_time_str)
            page_number += 1
            c.drawRightString(left_margin + 280, y, "Página: " + str(page_number))

            y -= 20

            c.drawString(left_margin + 10, y, "Descripción")       # Producto
            c.drawRightString(left_margin + 190, y, "Cantidad")  # Precio
            c.drawRightString(left_margin + 230, y, "Precio")  # Total
            c.drawRightString(left_margin + 280, y, "Total")  # Total

            y -= 20

            # Reiniciar la variable suma para la nueva página
            suma = 0

    if page_number > 1:

        y -=7

        c.drawRightString(left_margin + 280, y, "Suma de la pagina: {:.2f}".format(suma))

        y -= 13

        c.drawRightString(left_margin + 280, y, "Suma de paginas anteriores: {:.2f}".format(suma_total - suma))

        y -= 13

    else:

        y -= 7
        
    
    total = total_label.cget("text")  # Obtener el texto del total
    total = total.split(" ")[1]  # Quitar la palabra "Subtotal:"
    c.drawRightString(left_margin + 280, y, "Subtotal {:.2f}".format(float(total)))  # Imprimir el total

    y -= 13

    saldo = acreedor_deudor_var.get()  # Obtener el texto del saldo
    saldo = '0' if saldo == '' else saldo
    c.drawRightString(left_margin + 280, y, "Saldo: {:.2f}".format(float(saldo)))  # Imprimir el saldo

    pago_ef = efectivo_var.get()  # Obtener el texto del pago en efectivo
    pago_ef = '0' if pago_ef == '' else pago_ef
    c.drawString(left_margin + 10, y, "Efectivo: {:.2f}".format(float(pago_ef)))  # Imprimir el pago en efectivo

    y -= 13

    pago_ch = cheque_var.get()  # Obtener el texto del pago con cheque
    pago_ch = '0' if pago_ch == '' else pago_ch
    c.drawString(left_margin + 10, y, "Cheque: {:.2f}".format(float(pago_ch)))  # Imprimir el pago con cheque

    totalisimo = totalisimo_label.cget("text")  # Obtener el texto del totalisimo
    totalisimo = totalisimo.split(" ")[1]  # Quitar la palabra "Total:"
    c.drawRightString(left_margin + 280, y, "Total {:.2f}".format(float(totalisimo)))  # Imprimir el totalisimo

    y -= 13

    saldo_fin = saldo_final_label.cget("text")  # Obtener el texto del saldo final
    saldo_fin = saldo_fin.split(" ")[2]  # Quitar la palabra "Total:"
    c.drawRightString(left_margin + 280, y, "Saldo final: {:.2f}".format(float(saldo_fin)))  # Imprimir el saldo final

    # Finalizar y guardar el PDF
    c.save()

    # Abrir el PDF con el lector de PDF predeterminado
    #subprocess.Popen(["boleta.pdf"], shell=True)


    # Imprimir el PDF
    os.startfile("boleta.pdf", "print")
    
import os
from dbfread import DBF

current_dir = os.path.dirname(os.path.realpath(__file__))
dbf_file = os.path.join(current_dir, 'STOCK1.DBF')

stock = DBF(dbf_file, encoding='cp437')

 # Abrir y leer el archivo CLIPRO.DBF
dbf_file2 = os.path.join(current_dir, 'CLIPRO.DBF')
clipro = DBF(dbf_file2, encoding='cp437')


# Abrir y leer el archivo PROVE.DBF
dbf_file3 = os.path.join(current_dir, 'PROVE.DBF')
prove = DBF(dbf_file3, encoding='cp437')

cliente_label = tk.Label(window, text="Nombre del Cliente")
cliente_label.grid(row=0, column=3)
cliente_entry = ttk.Combobox(window, postcommand=autocomplete)
cliente_entry.grid(row=1, column=3)

boleta_label = tk.Label(window, text="Boleta")
boleta_label.grid(row=0, column=4)
boleta_entry = tk.Entry(window)
boleta_entry.grid(row=1, column=4)

numero_label = tk.Label(window, text="Numero de cliente")
numero_label.grid(row=0, column=2)
numero_entry = tk.Entry(window, textvariable=numero_var)
numero_entry.grid(row=1, column=2)

codi_label = tk.Label(window, text="codigo")
codi_label.grid(row=2, column=1)
codi_entry = ttk.Combobox(window, textvariable=codigo_var)
codi_entry.grid(row=3, column=1)

producto_label = tk.Label(window, text="Producto")
producto_label.grid(row=2, column=3)
producto_entry = ttk.Combobox(window, postcommand=autocomplete, width=50)
producto_entry.grid(row=3, column=2, columnspan=3)

existencia_label = tk.Label(window, text="Stock")
existencia_label.grid(row=4, column=2)
existencia_entry = tk.Entry(window, textvariable=existencias_var)
existencia_entry.grid(row=5, column=2)

tamaño_label = tk.Label(window, text="Tamaño")
tamaño_label.grid(row=4, column=3)
tamaño_entry = tk.Entry(window, textvariable=TAMAÑO_var)
tamaño_entry.grid(row=5, column=3)

def on_product_selection(event):
    global precio, precio_original
    # Obtiene el producto seleccionado
    selected_product = producto_entry.get()

    # Divide la cadena seleccionada en el nombre del producto y su tamaño
    selected_name, selected_size = selected_product.split('--', 1)

    # Busca el producto seleccionado en el stock y actualiza el precio
    for row in stock:
        if row['NOMBRE'] == selected_name and row['ENVASE'] == selected_size:
            precio_original = float(row['PRECIOB'])
            precio = precio_original
            # print(f"precio_original: {precio_original}")  # Impresión de depuración
            if precio_bonificado_var.get() == 1:
                precio = round(precio_original * 1.3, -1)  # Aumenta el precio en un 30% y redondea a la decena más cercana
            PRECIOB_var.set(precio)
            codigo_var.set(row['CODIGO'])
            existencias_var.set(row['EXISTENCIA'])
            TAMAÑO_var.set(row['ENVASE'])
            break
    cantidad_entry.focus_set()

def on_checkbox_change():
    global precio
    if precio_bonificado_var.get() == 1:
        precio = round(precio_original * 1.3, -1)  # Aumenta el precio en un 30% y redondea a la decena más cercana
    else:
        precio = precio_original  # Restaura el precio original
    # print(f"precio: {precio}")  # Impresión de depuración
    PRECIOB_var.set(precio)

# Vincula el evento de selección de la lista de sugerencias a la función on_product_selection
producto_entry.bind('<<ComboboxSelected>>', on_product_selection)

cantidad_label = tk.Label(window, text="Cantidad")
cantidad_label.grid(row=6, column=3)
cantidad_entry = tk.Entry(window)
cantidad_entry.grid(row=7, column=3)

var = tk.IntVar()
var.trace('w', update_price)

precio_label = tk.Label(window, text="Precio por unidad")
precio_label.grid(row=4, column=4)
precio_entry = tk.Entry(window, textvariable=PRECIOB_var)
precio_entry.grid(row=5, column=4)

checkbox = tk.Checkbutton(window, text='Publico',
variable=precio_bonificado_var, command=on_checkbox_change)
checkbox.grid(row=5, column=5)

acreedor_deudor_label = tk.Label(window, text="Saldo")
acreedor_deudor_label.grid(row=9, column=4)
acreedor_deudor_entry = tk.Entry(window, textvariable=acreedor_deudor_var)
acreedor_deudor_var.trace("w", lambda name, index, mode, sv=acreedor_deudor_var: calcular_total())
acreedor_deudor_entry.grid(row=10, column=4)

efectivo_label = tk.Label(window, text="Efectivo")
efectivo_label.grid(row=9, column=2)
efectivo_entry = tk.Entry(window, textvariable=efectivo_var)
efectivo_var.trace("w", lambda name, index, mode, sv=efectivo_var: calcular_total())
efectivo_entry.grid(row=10, column=2)

cheque_label = tk.Label(window, text="Cheque")
cheque_label.grid(row=9, column=1)
cheque_entry = tk.Entry(window, textvariable=cheque_var)
cheque_var.trace("w", lambda name, index, mode, sv=cheque_var: calcular_total())
cheque_entry.grid(row=10, column=1)

def on_client_selection(event):
    # Obtiene el producto seleccionado
    selected_client = cliente_entry.get()

    # Busca el producto seleccionado en el stock y actualiza el precio
    for row in clipro:
        if row['RAZON'] == selected_client:
            numero_var.set(row['NUMERO'])
            acreedor_var.set(row['ACREEDOR'])
            deudor_var.set(row['DEUDOR'])
            acreedor_deudor_var.set(float(acreedor_var.get()) - float(deudor_var.get()))
            break
    producto_entry.focus_set()

# Vincula el evento de selección de la lista de sugerencias a la función on_product_selection
cliente_entry.bind('<<ComboboxSelected>>', on_client_selection)

agregar_button = tk.Button(window, text="Agregar", command=agregar_producto)
agregar_button.grid(row=10, column=3)

factura_treeview = ttk.Treeview(window, columns=('Producto', 'Tamaño', 'Cantidad', 'Precio', 'Total'), show='headings')
factura_treeview.heading('Producto', text='Producto')
factura_treeview.heading('Tamaño', text='Tamaño')
factura_treeview.heading('Cantidad', text='Cantidad')
factura_treeview.heading('Precio', text='Precio')
factura_treeview.heading('Total', text='Total')
factura_treeview.grid(row=11, column=0, columnspan=6)
scrollbar = ttk.Scrollbar(window, orient="vertical", command=factura_treeview.yview)
scrollbar.grid(row=11, column=7, sticky="ns")
factura_treeview.configure(yscrollcommand=scrollbar.set)
factura_treeview.bind('<Delete>', borrar_fila)

total_label = tk.Label(window, text="Subtotal: 0")
total_label.configure(font=('Arial', 15))
total_label.grid(row=12, column=3)

totalisimo_label = tk.Label(window, text="Total: 0")
totalisimo_label.configure(font=('Arial', 15))
totalisimo_label.grid(row=12, column=4)

saldo_final_label = tk.Label(window, text="Saldo final: 0")
saldo_final_label.configure(font=('Arial', 15))
saldo_final_label.grid(row=12, column=5)

print_button = tk.Button(window, text="Imprimir boleta",
command=create_and_print_invoice)
print_button.grid(row=13, column=5)

window.mainloop()
