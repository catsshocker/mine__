import tkinter as tk
import random
import os


def shut():
    i = random.randint(1, 10)
    # print(i)
    if i >= 6:
        result.set("中槍了")
        os.system("""shutdown -s -t 15 -c "傻逼~~" """)
    else:

        result.set("幸運逃過一劫")


win = tk.Tk()

win.title("俄羅斯輪盤")
win.geometry("300x200")  # windows size
win.resizable(False, False)

txt = tk.Label(text="\n點擊板機 試試你的運氣\n\n\n")
txt.pack()

result = tk.StringVar()
result.set("")  # 初始值为空
load = tk.Label(win, textvariable=result)
load.pack()

btn = tk.Button(text="板機")
btn.config(command=shut)
btn.pack()

win.mainloop()
