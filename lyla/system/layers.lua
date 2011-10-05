-- layers settings

require 'table'

function layers_init()
  layers = {}
end

function layers_add(img, t)
  table.insert(layers, img)
  t = t or {}
  img.x = t.x or 0
  img.y = t.y or 0
  img.compile = t.compile or false
  img.texture = t.texture or t.texturize or false
  img.visible = t.visible or t.on or false
  if t.name then
    layers[t.name] = img
  end
end

