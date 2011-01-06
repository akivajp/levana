require 'lev/std'

frm = gui.frame()
sizer = gui.vsizer()
frm.sizer = sizer
txt = gui.text({p = frm})
sizer:add(txt,1)
txt = gui.text({p = frm})
sizer:add(txt,1)
sizer:fit(frm)

frm:show()
app:autoloop(frm)

