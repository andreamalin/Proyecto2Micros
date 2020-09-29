from tkinter import filedialog
#from tkinter import messagebox
from tkinter import *
from os import path
from PIL import Image, ImageTk

# Interface
root = Tk()
root['bg'] = 'black'
root.title('Message decrypter')
root.call('wm', 'iconphoto', root._w, PhotoImage(file='icon/skull.png'))
root.resizable(False,False)
root.geometry('950x3500')
frame = Frame()
frame.pack()
label = Label(root, text = 'DECRYPTER', font = ('Calibri Bold','40'), fg='white', bg='black')
label.place(x = 156, y = 5)



def open_file():
    file = filedialog.askopenfilename(filetype =(("Text files","*.txt"),("all files","*.*")))
    print(file)
  
    
    
Button(text = 'Abrir archivo', bg='black', fg='white', command = open_file).place(x = 275,y = 120)
imagen = ImageTk.PhotoImage(file='icon/ima.jpg')
lbl = Label(image = imagen).place(x=79,y=205)



#res = messagebox.askquestion('Message','¿Seguro que quiere salir del programa?')


root.mainloop()