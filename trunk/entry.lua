require 'lev/std'

frm = gui.frame()
sizer = gui.vsizer()
frm.sizer = sizer
txt = gui.text({p = frm})
sizer:add(txt,1)
txt = gui.text({p = frm})
sizer:add(txt,1)
--sizer:fit(frm)

snd = gui.frame()
player = media.player({p = snd})
player:loadlocal("./kami.mp3")
player:play()
frm.status = "volume: " .. player.volume .. "   AAA"

frm:fit()
frm:show()
app:autoloop(frm)

