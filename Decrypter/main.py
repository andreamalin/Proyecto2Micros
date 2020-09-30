from tkinter import filedialog
from tkinter import messagebox
from tkinter import *
from os import path
from PIL import Image, ImageTk

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



def open_file():
    file = filedialog.askopenfilename()
    print(file)
    label_0 = Label(root, text = file, font = ('Calibri Bold','40'), fg='white', bg='black')
    label_0.place(x = 300, y = 5)

def closing():
    if messagebox.askokcancel("Close","¿Seguro que quiere salir del programa?"):
        root.destroy()
    
    
Button(text = 'Abrir archivo', bg='black', fg='white', command = open_file).place(x = 275,y = 120)
img = Image.open("icon/ima.jpg")
img = img.resize((175,250), Image.ANTIALIAS)
my_image =  ImageTk.PhotoImage(img)

lbl = Label(image = my_image).place(x=229,y=205)

root.protocol("WM_DELETE_WINDOW", closing)


root.mainloop()