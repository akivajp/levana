require 'lev/lev'

mb = lev.menubar()
file = lev.menu()
id_exit = file:append(-1, '&Exit')
mb:append(file, '&File')

frm = lev.frame()
frm:set_menubar(mb)
frm:connect_menu(id_exit, function() app.msgbox('Bye Bye!'); os.exit() end)
frm:show()
app.autoloop()
os.exit()

