require 'lev'

app = lev.app.get()
while 1 do
--  app:yield()
  if app:get_keydown('ESC') then
    break
  elseif app.inrecord.left_down then
    print('Left Click!')
  else
    print(string.format('(X, Y) = (%d, %d)',app.input.x,app.input.y))
  end
  app:sleep(500)
end

