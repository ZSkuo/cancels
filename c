local LocalPlayer = game:GetService'Players'.LocalPlayer
local Character = LocalPlayer.Character or LocalPlayer.CharacterAdded:Wait()
local Humanoid;
local Mouse = LocalPlayer:GetMouse()

local FindFirstChild, WaitForChild, IsA, GetChildren = game.findFirstChild, game.waitForChild, game.IsA, game.GetChildren

local IsSetUp, Names = {}, {
    'Glock', 'Shotty', 'Uzi'
}

local Connect = game.Loaded.Connect

local PlayingAnim = false
local SetupClient = function(Gun)
    local Connections = {}; repeat
        Connections = getconnections(Gun.Activated)
        wait()
    until #Connections > 0

    for I, V in pairs(Connections) do
        V:Disable()
    end

    repeat
        wait()
    until Humanoid and Humanoid.Health > 0

    local Fires = WaitForChild(Gun, 'Fires')

    local Remote = WaitForChild(Gun, 'Shoot')
    local FireServer = Remote.FireServer

    local Loaded = Humanoid:LoadAnimation(Fires)
    local Play = Loaded.Play

    local GName = Gun.Name

    Connect(Gun.Activated, function()
        spawn(function()
            FireServer(Remote, Mouse.Hit)
        end)
        spawn(function()
            if PlayingAnim then return end
            PlayingAnim = true
            Play(Loaded, 0.1, 1, 2)
            Loaded.Stopped:Wait()
            if GName == 'Shotty' then
                wait(0.16)
            end
            PlayingAnim = false
        end)
    end)
end

local OnChildAdded;

local onCharacterAdded = function(newCharacter)
    Character = newCharacter
    Humanoid = WaitForChild(newCharacter, 'Humanoid')

    if OnChildAdded then
        OnChildAdded:Disconnect()
        OnChildAdded = nil
    end

    OnChildAdded = newCharacter.ChildAdded:Connect(function(Tool)
        if IsA(Tool, 'Tool') and table.find(Names, Tool.Name) and not table.find(IsSetUp, Tool) then
            SetupClient(Tool); IsSetUp[#IsSetUp+1] = Tool
        end
    end)
end

onCharacterAdded(Character)
LocalPlayer.CharacterAdded:Connect(onCharacterAdded)
