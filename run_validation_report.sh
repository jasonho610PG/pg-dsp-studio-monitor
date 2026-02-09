#!/bin/bash
# Validation Report Generator Wrapper Script
# pg-dsp-studio-monitor

echo "========================================"
echo "Validation Status Report Generator"
echo "pg-dsp-studio-monitor"
echo "========================================"
echo ""

# Run the Python script
python3 /Users/jasonho610/Desktop/pg-dsp-studio-monitor/generate_validation_report.py

# Check if successful
if [ $? -eq 0 ]; then
    echo ""
    echo "Report generated successfully!"
    echo "View report: /Users/jasonho610/Desktop/pg-dsp-studio-monitor/reports/validation-report.md"
else
    echo ""
    echo "ERROR: Report generation failed"
    exit 1
fi
