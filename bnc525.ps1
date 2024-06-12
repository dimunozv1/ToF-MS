# Specify the COM port name. Update this with your device's COM port.
$comPort = "COM8"
$filePath = ".\TOF_Trigger_scheme.txt"


# Check if there is at least one argument provided
if ($args.Length -gt 0) {
    $Programm_Runtime_ms = $args[0]
    $Period_s = $args[1]
    Write-Host "Received runtime variable: $Programm_Runtime_ms"
} else {
    Write-Host "No runtime variable provided."
}
$FormattedPeriod = $Period_s.ToString()
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
    
    foreach ($command in $commands) {
        $serialPort.WriteLine($command)
        Write-Host "Command: $command" #Writes the command out to the terminal
        Start-Sleep -Milliseconds 100  # Wait for a short time between commands if necessary
        # Read the response from the device and display it
        $response = $serialPort.ReadExisting()
        Write-Host "Response: $response" #Writes the response from bnc 525 to the terminal
    }
    $serialPort.WriteLine(":PULSE0:PER $Period_s`r`n")
    Write-Host "Command: :PULSE0:PER $Period_s`r`n" #Writes the command out to the terminal
    Start-Sleep -Milliseconds 100
    $response = $serialPort.ReadExisting()
    Write-Host "Response: $response" #Writes the response from bnc 525 to the terminal
    $serialPort.WriteLine(":PULSE0:STATE ON`r`n")
    Write-Host "Command: :PULSE0:STATE ON"
    $response = $serialPort.ReadExisting()
    Write-Host "Response: $response"
    Start-Sleep -Milliseconds $Programm_Runtime_ms
    $serialPort.WriteLine(":PULSE0:STATE OFF`r`n")
    Write-Host "Command: :PULSE0:STATE OFF"
    $response = $serialPort.ReadExisting()
    Write-Host "Response: $response"
    #Close the serial port
    $serialPort.Close()
    Exit
}
catch {
    Write-Host "Error: $_.Exception.Message"
}
