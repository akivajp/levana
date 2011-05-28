require 'lev/std'

frm = gui.frame.create {}
sizer = gui.vsizer()
frm.sizer = sizer
txt = gui.text {p = frm}
sizer:add(txt,1)
txt = gui.text({p = frm})
sizer:add(txt,1)
sizer:fit(frm)

mixer = lev.sound.mixer.create()
mixer(1):open('kami.ogg')

frm:fit()
frm:show()
app:autoloop(frm)

