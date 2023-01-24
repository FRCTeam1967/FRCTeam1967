#!/bin/sh

echo "Fetching 5th symphony..."
curl https://bitmidi.com/uploads/1674.mid --output midi/Beethoven-5thSymphony.midi

echo "Fetching Moonlight Sonata..."
curl https://bitmidi.com/uploads/16752.mid --output midi/Beethoven-MoonlightSonata.midi

echo "Fetching NGYU... "
curl https://bitmidi.com/uploads/79827.mid --output midi/RickAstley-NeverGonnaGiveYouUp.midi

echo "Fetching YMCA..."
curl https://bitmidi.com/uploads/112155.mid --output midi/VillagePeople-YMCA.midi
