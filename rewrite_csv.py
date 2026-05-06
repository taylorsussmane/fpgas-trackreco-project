import matplotlib.pyplot as plt
import csv
import numpy as np
import pandas as pd

# Load file with pandas
df = pd.read_csv('trackerInputs_events.csv')

# Remove duplicate from CSV
df_clean = df.drop_duplicates(subset=['event_id','layer'], keep='first')

# Save File
df_clean.to_csv('trackerInputs_SingleHitEvents.csv', index=False)

