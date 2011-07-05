require 'lev'

app = lev.app.get()

frm = lev.gui.frame()
mb = lev.gui.menubar()
menu = lev.gui.menu()
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
  frm:close()
end

frm:set_menubar(mb)
frm:show()

app:autoloop()

