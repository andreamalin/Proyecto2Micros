from tkinter import filedialog
from tkinter import messagebox
from tkinter import *
from os import path
from PIL import Image, ImageTk
import os

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


def open_file():
	global route
	file = filedialog.askopenfilename(filetypes=[("Text files","*.txt")])
	route = file

def read():
	if (route != ''):
		global line
		file = open(route,'r')
		line = file.readlines()
		print(line)
		file.close
		if(line != []):
			write(line)
		else:
			messagebox.showinfo(message = 'Archivo vacio', title = 'Contenido faltante')
	else:
		messagebox.showinfo(message = 'Busque primero el archivo', title = 'Ruta faltante')

def write(line):

	x = line[0].split(',')
	x.pop(len(x)-1)
	print(x)



def closing():
    if messagebox.askokcancel("Close","¿Seguro que quiere salir del programa?"):
        root.destroy()
    
    
Button(text = 'Abrir archivo', bg='black', fg='white', command = open_file).place(x = 275,y = 105)
Button(text = 'Desencriptar', bg='black', fg='white', command = read).place(x = 275,y = 150)

img = Image.open("icon/ima.jpg")
img = img.resize((175,250), Image.ANTIALIAS)
my_image =  ImageTk.PhotoImage(img)

lbl = Label(image = my_image).place(x=229,y=205)

root.protocol("WM_DELETE_WINDOW", closing)


root.mainloop()