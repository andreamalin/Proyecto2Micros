from tkinter import filedialog
from tkinter import *
from os import path 

# Interface
root = Tk()
root.title('Decrypter')
root.call('wm', 'iconphoto', root._w, PhotoImage(file='icon/skull.png'))
root.resizable(False,False)
root.geometry('650x350')
frame = Frame()
frame.pack()
label = Label(root, text = 'DECRYPTER', font = ('Calibri','18'))
label.place(x = 275, y = 5)

def open_file():
	file = filedialog.askopenfilename()
	print(file)

Button(text = 'Abrir archivo', command = open_file).place(x = 10,y = 10)


root.mainloop()