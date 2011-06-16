require 'lev.std'

app = lev.app.get()

frm = lev.gui.frame.create {}
sizer = lev.gui.vsizer.create()
frm.sizer = sizer
txt = lev.gui.textbox.create(frm)
sizer:add(txt,0)
html = lev.gui.textedit.create(frm)
sizer:add(html,1)
--sizer:fit(frm)
--frm:fit()
frm:show()

txt.onkeydown = function(e)
  print("id  = " .. e.id)
  print("key = " .. e.key)
  e:skip()
end

app:autoloop(frm)

