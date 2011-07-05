require 'lev.std'

frm = lev.gui.frame()
frm:show()
--frm.onkeydown = function(o)
--  print(o.key)
--end

app.on_keydown = function(o)
  print(o.key)
end

--app.onkeydown = onkeydown
--print (app:set_onkeydown(onkeydown))

frm.on_close = function(e)
  local res = lev.gui.msgbox {'Exiting...', style = "ok|cancel"}
  if res then e:skip() end
end

app:autoloop()

