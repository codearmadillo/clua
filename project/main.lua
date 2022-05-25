local width = 960
local ratio = 0.75
local factor = 25;

function updateWindow()
    clua.window.setWidth(width)
    clua.window.setHeight(width * ratio)
end

function clua.start()
    clua.window.setTitle('Clua game')
    updateWindow()
end
function clua.keyboard.keyPressed(key)
    if (key == 'arrow_up') then
        width = width + factor;
        updateWindow();
    elseif (key == 'arrow_down') then
        width = width - factor;
        updateWindow();
    end
end