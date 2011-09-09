require 'lev.gui'

app = lev.app()

frm = lev.gui.frame()
frm:show()

frm.on_close = function(e)
  if st then
    st:destroy()
    st = nil
  end
  e:skip()
end

st = lev.gui.systray()
st.menu_generator = function()
  local menu = lev.gui.menu()
  id = menu:append('Exit(&X)')
  menu.fmap[id] = function()
    lev.gui.msgbox('Bye!')
    frm:close()
  end
  return menu
end

app:run()

