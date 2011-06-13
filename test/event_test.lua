require 'lev.std'

frm = lev.gui.frame.create()
frm:show()
frm.onkeydown = function(o)
  print(o.key)
end

app.onkeydown = function(o)
  print(o.key)
end

--app.onkeydown = onkeydown
--print (app:set_onkeydown(onkeydown))

app:autoloop()

