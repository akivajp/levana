require 'lev'

app = lev.app.get()
app:track_mouse()
app.fps = 2
while 1 do
--  app:yield()
  if app:get_keydown('ESC') then
    break
  elseif app.inrec.left_down then
    print('Left Click!')
  else
    print(string.format('(X, Y) = (%d, %d)',app.input.x,app.input.y))
  end
  app:wait()
end

