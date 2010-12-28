require 'lev/all'

mb = lev.menubar()
file = lev.menu()
file:append {str = '&Exit', func = function() frm:close() end}
mb:append(file, '&File')

tray = lev.systray()
sysmenu = lev.menu()
sysmenu:append {str = '&Test', func = function() app:msgbox("bye systray"); frm:close() end}
tray:setmenu(sysmenu)

frm = lev.frame()
frm:setmenubar(mb)
frm:show()
while frm:exists() do
  app:yield()
end

