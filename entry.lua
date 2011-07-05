require 'lev.std'

app = lev.app.get()

frm = lev.gui.frame {}
sizer = lev.gui.vsizer()
frm.sizer = sizer
txt = lev.gui.textbox(frm)
sizer:add(txt,0)
txt2 = lev.gui.textbox(frm, {style = "multiline"})
sizer:add(txt2,1)
--sizer:fit(frm)
--frm:fit()
frm:show()

txt.on_keydown = function(e)
  frm.status = e.key .. " was pressed"
--  txt2.v = "key = " .. e.key .. "\n"
  e:skip()
end

app:autoloop(frm)

