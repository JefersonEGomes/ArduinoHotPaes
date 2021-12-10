import './App.css';
import Paper from '@mui/material/Paper';
import AppBar from "@mui/material/AppBar";
import Toolbar from "@mui/material/Toolbar";
import Typography from '@mui/material/Typography';
import Grid from '@mui/material/Grid';
import Container from "@mui/material/Container";
import React, { useEffect, useState } from "react";
import api from "./services/api";
import Chart from "../src/components/Charts";

export default function App() {

  const [temp, setTemp] = useState();

  // Utilização mais simples de busca axios
  useEffect(() => {
    api
      .get("/data_temp")
      .then((response) => setTemp(response.data))
      .catch((err) => {
        console.error("ops! ocorreu um erro" + err);
      });
  }, []);

  return (
    <div className="App" >
      <AppBar
        position="absolute"
      >
        <Toolbar>
          <Typography
            component="h1"
            variant="h6"
            color="white"
            noWrap
          >
            &#128293; HOT PÃES &#x1F956;
          </Typography>
        </Toolbar>
      </AppBar>
      <main className="App-header" sx={{ padding: 4 }}>
        <div />
        <Container maxWidth="md" sx={{ padding: 4, marginLeft: 20 }}>
          <Grid container spacing={4}>
            {/* Grafico */}
            <Grid item xs={12} md={8} lg={9}>
              <Paper sx={{
                height: 260,
                paddingBottom: 8,
                paddingRight: 4,
                marginTop: 12,
                flexDirection: "column"
              }}>
                <Chart />
              </Paper>
            </Grid>
            {/* Temperatura */}
            <Grid item xs={12} md={4} lg={3} marginTop={12}>
              <Paper sx={{ p: 2, margin: 'auto', minWidth: 435, height: 275, flexGrow: 1, padding: 3 }}>
                <Typography sx={{ fontSize: 14 }}>Temperatura (°C)</Typography>
                <Grid container>
                  <Grid item xs={12} sm container sx={{
                    alignItems: 'center', justifyContent: 'center',
                    direction: "column"
                  }}>
                    <Typography sx={{ fontSize: 25 }}>&#127777;</Typography>
                    <Typography sx={{ marginTop: 20, marginBottom: 4, fontSize: 50 }}>
                      {temp?.medida}
                    </Typography>
                    <Grid container spacing={2}>
                      <Grid item xs container direction="row" spacing={4}>
                        <Grid item xs={6} sm container sx={{ alignItems: 'center', justifyContent: 'center' }}>
                          <Typography>
                            Id:
                          </Typography>
                        </Grid>
                        <Grid item xs={6} sm container sx={{ alignItems: 'center', justifyContent: 'center' }}>
                          <Typography>
                            Status:
                          </Typography>
                        </Grid>
                      </Grid>
                    </Grid>
                  </Grid>
                </Grid>
              </Paper>
            </Grid>
          </Grid>
        </Container>
      </main>
      <Container sx={{ padding: 2, marginTop: 3, backgroundColor: "white", alignSelf: "flex-end" }}>
        <Typography variant="body2" color="textSecondary" align="center">
          {"Copyright © "}
          <p>Projeto desenvolvido pela equipe Hot Pães para a disciplina de Automação Comercial</p>
          {new Date().getFullYear()}
        </Typography>
      </Container>
    </div>
  );
}
