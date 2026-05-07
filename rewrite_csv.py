import matplotlib.pyplot as plt
import csv
import numpy as np
import pandas as pd

# Load file with pandas
df = pd.read_csv('trackerInputs_events.csv')

# Remove duplicate from CSV
df_clean = df.drop_duplicates(subset=['event_id','layer'], keep='first')

# Add Residuals to each hit
def create_variation(row):
    new_row = row.copy()
    new_row['x']=row['x']*1.1 #Give it a 10% shift in the same direction
    new_row['y']=row['y']*1.1
    new_row['t']=row['t']*1.1
    return new_row

# Save File
variations_df=df_clean.apply(create_variation, axis=1)
extra_df = pd.concat([df_clean, variations_df]).sort_index(kind='merge').reset_index(drop=True)
extra_df.to_csv('trackerInputs_SingleHitEvents.csv', index=False)

