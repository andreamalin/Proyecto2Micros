from tkinter import filedialog
from tkinter import messagebox
from tkinter import *
from os import path
from PIL import Image, ImageTk
import os
#import gmpy

# Interface
root = Tk()
root['bg'] = 'black'
root.title('Message decrypter')
root.call('wm', 'iconphoto', root._w, PhotoImage(file='icon/skull.png'))
root.resizable(False,False)
root.geometry('600x500')
frame = Frame()
frame.pack()
label = Label(root, text = 'DECRYPTER', font = ('Calibri Bold','40'), fg='white', bg='black')
label.place(x = 156, y = 5)

route = ''
line = ''
key = 18
    

def open_file():
    global route
    file = filedialog.askopenfilename(filetypes=[("Text files","*.txt")])
    route = file

def read():
    if (route != ''):
        global line
        file = open(route,'r')
        line = file.readlines()
        file.close
        if(line != []):
            write(line)
        else:
            messagebox.showinfo(message = 'Archivo vacio', title = 'Contenido faltante')
    else:
        messagebox.showinfo(message = 'Se debe de seleccionar un archivo', title = 'Ruta faltante')

def write(line):
    final = []
    x = line[0].split(',')
    x.pop(len(x)-1)
    for i in range(len(x)):
        decrypted = ''
        for j in range(len(x[i])):
            decrypted += decrypt(x[i][j])
        final.append(decrypted)
    messagebox.showinfo(message = 'Indicar Ruta de Guardado', title = 'Requerimiento')
    route = filedialog.asksaveasfilename(filetypes=[("Text files","*.txt")])
    file = open(route,'w')
    for i in range(len(final)):
        file.write(str(final[i])+os.linesep)
    file.close()
    messagebox.showinfo(message = 'Desencriptado Finalizado', title = 'Completo')



def decrypt(element):
    decrypted = 0
    element = ord(element)
    a = element-97
    #invmod = int(gmpy.divm(a,1,26))
    if ((element < 97) & (element > 75)):
        #decrypted = invmod - 26*2 + (97 - key)
        decrypted = a + 26 - 26*2 + (97 - key)
    else:
        #decrypted = invmod - 26 + (97 - key)
        decrypted = a + 26 - 26 + (97 - key)
    decrypted = chr(decrypted)

    return decrypted

def closing():
    if messagebox.askokcancel("Close program","¿Seguro que quiere salir del programa?"):
        root.destroy()
    
    
Button(text = 'Abrir archivo', bg='black', fg='white', command = open_file).place(x = 275,y = 105)
Button(text = 'Desencriptar', bg='black', fg='white', command = read).place(x = 275,y = 150)

img = Image.open("icon/ima.jpg")
img = img.resize((175,250), Image.ANTIALIAS)
my_image =  ImageTk.PhotoImage(img)

lbl = Label(image = my_image).place(x=229,y=205)

root.protocol("WM_DELETE_WINDOW", closing)


root.mainloop()