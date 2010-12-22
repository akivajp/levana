require 'lev/all'

mb = lev.menubar()
file = lev.menu()
id_exit = file:append {str = '&Exit'}
mb:append(file, '&File')

tray = levana.systray()
tray:set_menu(file)
--tray:set_icon(levana.icon('./levana.xpm'), 'Levana Application')

frm = lev.frame()
frm:set_menubar(mb)
frm:connect_menu(id_exit, function() app:msgbox('Bye Bye!'); frm:close() end)
frm:show()
app:autoloop()

