# Specify the COM port name. Update this with your device's COM port.
$comPort = "COM3"
$filePath = "C:\Users\Administrator\Documents\Diana\xtdc_babel-main\lib\x64\Debug\single_shot.txt"

# Function to read commands from the file
function ReadCommandsFromFile($filePath) {
    if (Test-Path $filePath) {
        $commands = Get-Content -Path $filePath
        $commands = $commands | ForEach-Object { $_.Trim() }
        return $commands
    } else {
        throw "File not found: $filePath"
    }
}

try {
    # Open the COM port for communication
    $serialPort = new-Object System.IO.Ports.SerialPort $comPort, 9600, None, 8, one

    # Open the serial port
    $serialPort.Open()

    # Read SCPI commands from the file
    $commands = ReadCommandsFromFile $filePath
    Start-Sleep -Milliseconds 5000
    foreach ($command in $commands) {
        $serialPort.WriteLine($command)
        Write-Host "Command: $command" #Writes the command out to the terminal
        Start-Sleep -Milliseconds 100  # Wait for a short time between commands if necessary
        # Read the response from the device and display it
        $response = $serialPort.ReadExisting()
        Write-Host "Response: $response" #Writes the response from bnc 525 to the terminal
    }
    Start-Sleep -Milliseconds 10000
    $serialPort.WriteLine(":PULSE0:STATE OFF`r`n")
    Write-Host "Command: :PULSE0:STATE OFF"
    $response = $serialPort.ReadExisting()
    Write-Host "Response: $response"
    #Close the serial port
    $serialPort.Close()
}
catch {
    Write-Host "Error: $_.Exception.Message"
}
