require 'lev'

app = lev.app.get()

frm = lev.gui.frame.create()
mb = lev.gui.menubar.create()
menu = lev.gui.menu.create()
mb:append(menu, 'Greetings(&G)')
id = menu:append('Morning(&M)')
menu.fmap[id] = function()
  lev.gui.msgbox('Guten Morgen!')
end
id = menu:append('Afternoon(&A)')
menu.fmap[id] = function()
  lev.gui.msgbox('Guten Tag!')
end
id = menu:append('Evening(&E)')
menu.fmap[id] = function()
  lev.gui.msgbox('Guten Abend!')
end

frm:set_menubar(mb)
frm:show()

app:autoloop()

