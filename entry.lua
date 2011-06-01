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

mixer[1]:play('kami.ogg')

--frm.onkeydown = function(e)
--  print(e.key)
--end

frm:fit()
frm:show()
--app:autoloop(frm)
while frm.is_valid do
  app:yield()
  if (app:get_keystate('SHIFT')) then
    if (app:get_keystate('A')) then print('A') end
  else
    if (app:get_keystate('A')) then print('a') end
  end
  if (app:get_keystate("\t")) then print('TAB') end
  if (app:get_keystate('ESC')) then frm:close() end
end

