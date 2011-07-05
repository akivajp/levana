require 'lev.sound'
require 'os'

if not arg[1] then
  if arg[-1] then
    cmd = arg[-1] .. ' ' .. arg[0]
  else
    cmd = arg[0]
  end
  print('usage: ' .. cmd .. ' filepath')
  os.exit()
end

mixer = lev.sound.mixer()
for i,j in ipairs(arg) do
  mixer[i]:play(j)
  while (mixer[i].is_playing) do end
end

