require 'lev.std'

app = lev.app.get()

frm = lev.gui.frame.create {}
sizer = lev.gui.vsizer.create()
frm.sizer = sizer
txt = lev.gui.textbox.create(frm)
sizer:add(txt,0)
txt2 = lev.gui.textedit.create(frm)
sizer:add(txt2,1)
--sizer:fit(frm)
--frm:fit()
frm:show()

txt.onkeydown = function(e)
  txt2.v = "key = " .. e.key .. "\n"
  e:skip()
end

app:autoloop(frm)

