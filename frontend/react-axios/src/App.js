import './App.css';
import Paper from '@mui/material/Paper';
import Typography from '@mui/material/Typography';
import Grid from '@mui/material/Grid';

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <p>
        &#128293; HOT PÃES &#x1F956; 
        </p>
          <Paper sx={{ p: 2, margin: 'auto', minWidth: 435, maxHeight:150 , flexGrow: 1, padding: 3 }}>
      <Grid container>
     
        <Grid item xs={12} sm container sx={{alignItems:'center', justifyContent:'center',
  direction:"column"}}>
              <Typography sx={{ fontSize: 25 }}>&#127777;</Typography>
              <Typography sx={{ marginTop: 3, marginBottom: 5, fontSize: 50 }}>
                23 °C
              </Typography>
            <Grid container spacing={2}>
          <Grid item xs container direction="row" spacing={4}>
            <Grid item xs={6} sm container sx={{alignItems:'center', justifyContent:'center'}}>
              <Typography>
                Id:
              </Typography>
            </Grid>
            <Grid item xs={6} sm container sx={{alignItems:'center', justifyContent:'center'}}>
                <Typography>
                  Status:
                </Typography>
              </Grid>
          </Grid>
          </Grid>
        </Grid>
      </Grid>
    </Paper>
          <div className="App-link">
            <p>Trabalho da equipe Hot Pães, apresentado para a disciplia de Automação Comercial da Faculdade Senac Pernambuco (2021.2)</p>
          </div>
      </header>
    </div>
  );
}

export default App;
