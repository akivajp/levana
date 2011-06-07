require 'lev/std'

app = lev.app.get()
--mixer = lev.sound.mixer.create()

frm = lev.gui.frame.create {}
sizer = lev.gui.vsizer.create()
frm.sizer = sizer
txt = lev.gui.textbox.create {p = frm}
sizer:add(txt,1)
txt = lev.gui.textbox.create {p = frm}
sizer:add(txt,1)
sizer:fit(frm)

txt.onkeydown = function(e)
  print("id  = " .. e.id)
  print("key = " .. e.key)
end

--mixer[1]:play('kami.ogg')

frm:fit()
frm:show()
--app:autoloop(frm)

function idle()
  i = 1
  while i do
    print("i = " .. i)
    i = i + 1
    coroutine.yield()
  end
end
co = coroutine.create(idle)

--app.onidle = function()
--  coroutine.resume(co)
--end

while frm.is_valid do
  app:yield()
  if (app:get_keydown('SHIFT')) then
    if (app:get_keydown('A')) then print('A') end
  else
    if (app:get_keydown('A')) then print('a') end
  end
  if (app:get_keydown("\t")) then print('TAB') end
  if (app:get_keydown('ESC')) then
    if (not send_close) then
      frm:close()
      print 'ESCAPING!'
      send_close  = true
    end
  end
end

print '[END OF FILE]'

