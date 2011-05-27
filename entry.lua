require 'lev/std'

frm = gui.frame.new()
sizer = gui.vsizer()
frm.sizer = sizer
txt = gui.text({p = frm})
sizer:add(txt,1)
txt = gui.text({p = frm})
sizer:add(txt,1)
sizer:fit(frm)

--snd = gui.frame.new()
--player = media.player({p = snd})
--player:loadlocal("./kami.ogg")
--player.volume = 0.9
--player:play()
--frm.status = "volume: " .. player.volume
lev.sound.play("kami.ogg", 0)

frm:fit()
frm:show()
app:autoloop(frm)

