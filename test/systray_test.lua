require 'lev'

app = lev.app.get()

frm = lev.gui.frame()
frm:show()

st = lev.gui.systray()
st.menu_generator = function()
  menu = lev.gui.menu()
  id = menu:append('Exit(&X)')
  menu.fmap[id] = function()
    lev.gui.msgbox('Bye!')
    frm:close()
  end
  return menu
end

app:autoloop(frm)

