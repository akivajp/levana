require 'lev'

app = lev.app.get()

frm = lev.gui.frame.create()
frm:show()

st = lev.gui.systray.create()
st.menu_generator = function()
  menu = lev.gui.menu.create()
  id = menu:append('Exit(&X)')
  menu.fmap[id] = function()
    lev.gui.msgbox('Bye!')
  --  frm:close()
  end
  return menu
end
--st:set_menu(menu)

--app:autoloop(frm)
app:run(true)

