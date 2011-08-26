require 'lev.std'

frm = lev.gui.frame()
text = lev.gui.textbox(frm, 320, 240, {style = 'multi'})
text.on_keydown = function(e)
  print(e.key)
  e:skip()
end
frm:fit()
frm:show()

frm.on_close = function(e)
  local res = lev.gui.msgbox {'Exiting...', style = "ok|cancel"}
  if res then e:skip() end
end

app:run()

